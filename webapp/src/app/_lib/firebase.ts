import { initializeApp } from 'firebase/app';
import { getAuth } from 'firebase/auth';
import { getFirestore } from 'firebase/firestore';

// Your web app's Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyDzqjQTFz9MuvBVpUR7y_56q15yMaYkMb8",
    authDomain: "watering-system-e026c.firebaseapp.com",
    projectId: "watering-system-e026c",
    storageBucket: "watering-system-e026c.firebasestorage.app",
    messagingSenderId: "58044264820",
    appId: "1:58044264820:web:cea10aabc8bfbfdc323c21"
  };
  

// Initialize Firebase
const app = initializeApp(firebaseConfig);

// Firebase services
const auth = getAuth(app);
const db = getFirestore(app);

export { auth, db };