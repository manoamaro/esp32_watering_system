/**
 * Import function triggers from their respective submodules:
 *
 * import {onCall} from "firebase-functions/v2/https";
 * import {onDocumentWritten} from "firebase-functions/v2/firestore";
 *
 * See a full list of supported triggers at https://firebase.google.com/docs/functions
 */

import {onRequest} from "firebase-functions/v2/https";
import * as logger from "firebase-functions/logger";
import express, {Request, Response, NextFunction} from "express";
import crypto, {randomBytes} from "node:crypto";
import {defineSecret} from "firebase-functions/params";

// The Firebase Admin SDK to access Firestore.
import {initializeApp} from "firebase-admin/app";
import {getAuth} from "firebase-admin/auth";
import {getFirestore} from "firebase-admin/firestore";


const deviceToken = defineSecret("DEVICE_TOKEN");
const DEVICE_TOKEN_VALIDITY_MILLISECONDS = 60000;

const firebaseApp = initializeApp();
const auth = getAuth(firebaseApp);
const db = getFirestore(firebaseApp);

const app = express();

const generateToken = (timestamp: number): string => {
  const minutes = Math.floor(timestamp / DEVICE_TOKEN_VALIDITY_MILLISECONDS);
  const data = `${deviceToken.value()}:${minutes}`;
  return crypto.createHash("sha256").update(data).digest("hex");
};


// header token middleware to check if the token is valid
const deviceTokenMiddleware = (req: Request, res: Response, next: NextFunction) => {
  const timestamp = Date.now();
  const {token} = req.headers;
  if (!token || token !== generateToken(timestamp)) {
    res.status(401).send("Unauthorized");
  } else {
    next();
  }
};

const deviceMiddleware = async (req: Request, res: Response, next: NextFunction) => {
  const {deviceId} = req.params;
  if (!deviceId) {
    res.status(400).send("Device id is required");
    return;
  }
  const collection = db.collection("devices");
  const deviceRef = collection.doc(deviceId);
  const deviceSnapshot = await deviceRef.get();
  res.locals.deviceRef = deviceRef;
  res.locals.deviceSnapshot = deviceSnapshot;
  next();
};

// auth middleware to check if the user is authenticated
const authMiddleware = async (req: Request, res: Response, next: NextFunction) => {
  const idToken = req.headers["authorization"] && req.headers["authorization"].split("Bearer ")[1];
  if (!idToken) {
    res.status(401).send("Unauthorized");
    return;
  }
  try {
    const decodedToken = await auth.verifyIdToken(idToken);
    res.locals.user = decodedToken;
    next();
  } catch (error) {
    logger.error("Error while verifying token", error);
    res.status(401).send("Unauthorized");
  }
};

app.get("/device/:deviceId", deviceTokenMiddleware, deviceMiddleware, async (req: Request, res: Response) => {
  const {deviceSnapshot} = res.locals;
  if (!deviceSnapshot.exists) {
    res.status(404).send("Device not found");
    return;
  }
  const deviceData = deviceSnapshot.data();
  res.send(deviceData);
});

app.post("/device/:deviceId/enroll/token", deviceTokenMiddleware, deviceMiddleware,
  async (req: Request, res: Response) => {
    const {deviceSnapshot, deviceRef} = res.locals;
    let enrollmentToken;
    if (deviceSnapshot.exists) {
    // device already enrolled, check if enrollment token is still valid, if not, generate a new one
      enrollmentToken = deviceSnapshot.get("enrollmentToken");
    } else {
    // generate a new enrollment token
      enrollmentToken = randomBytes(64).toString("base64");
      await deviceRef.set({
        enrollmentToken,
        enrolledAt: new Date(),
      });
    }
    res.send({enrollmentToken});
  });

app.get("/user", authMiddleware, (req: Request, res: Response) => {
  res.send("User authenticated");
});

app.post("/user/device/enroll", authMiddleware, async (req: Request, res: Response) => {
  const {deviceId, enrollmentToken} = req.body;
  if (!deviceId || !enrollmentToken) {
    res.status(400).send("Device id and enrollment token are required");
    return;
  }
  const collection = db.collection("devices");
  const deviceDocRef = collection.doc(deviceId);
  const deviceDoc = await deviceDocRef.get();
  if (!deviceDoc.exists) {
    res.status(404).send("Device not found");
    return;
  }
  const storedEnrollmentToken = deviceDoc.get("enrollmentToken");
  if (enrollmentToken !== storedEnrollmentToken) {
    res.status(401).send("Unauthorized");
    return;
  }
  const userId = deviceDoc.get("userId");
  if (userId) {
    res.status(409).send("Device already enrolled");
    return;
  }
  const {user} = res.locals;
  // generate a api key for the device
  const customClaims = {
    role: 'device',
    permissions: ['create', 'read', 'update'],
  };
  const customToken = await auth.createCustomToken(deviceId, customClaims);
  // store the api key in the database along with the user id
  await deviceDocRef.set({customToken, userId: user.uid}, {merge: true});
  res.send({customToken});
});

exports.app = onRequest({secrets: [deviceToken], maxInstances: 2, timeoutSeconds: 10}, app);

// Start writing functions
// https://firebase.google.com/docs/functions/typescript

// export const helloWorld = onRequest((request, response) => {
//   logger.info("Hello logs!", {structuredData: true});
//   response.send("Hello from Firebase!");
// });
