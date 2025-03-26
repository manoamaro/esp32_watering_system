#pragma once

#include "pgmspace.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <mbedtls/md.h>

namespace FB
{
    const char API_KEY[] = "AIzaSyDzqjQTFz9MuvBVpUR7y_56q15yMaYkMb8";
    const char FIREBASE_PROJECT_ID[] = "watering-system-e026c";
    const char FIREBASE_CLIENT_EMAIL[] = "firebase-adminsdk-nnyur@watering-system-e026c.iam.gserviceaccount.com";
    const char FIREBASE_FUNCTIONS_URL[] = "https://watering-system-e026c.firebaseio.com";
    const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQC2PzQRZ98ek3im\nCQwQFENIYbC8LcDE8JaTLmqqs/TmFG9bAqHlPXMaJJ8K1ISNCbktl/4csi8FSc+p\nYWpn+XY0tiDNM9+GXBT4c0NYz7/aFIZR7CGUhwZTkNWM7haTwDCXtqKdlO1lnh4r\n0ybRyuHRf1hMdCq+S5I61x1yTN3Sf8zyUb0GJcJVAWZOYUo0kXWLUpCLaUu79RgZ\nWqA5z7uNlZPeZwxOt8pnx4VGayD/1Q80apWZMXgSVr7Q4FeS+v+ESC8Hm2DfAN+X\nflJONaOUBzTU8H64I5CVlvnbcc8MY/yvfk2+gdZ01+Al1MfgRFdvAaNvfa1ZjJcc\nCv4FO6F/AgMBAAECggEAVq2k1Ap1IhO9tCplZFhez9Q2vKl+H6zV0NaowNuqRKu/\ncqfD3EXnsq1sPzhoerP/MOIzZ+6PUpKSg9rjQuRxBg0BeNeNHlg5WRlpFJpqgwxz\n/g6ywfOeXKxsBQVQE6G6+s1QdmZYqpYTf4CD3BAEayGIG4S8t5k1yWp6hNxuPCjg\nzPXzdKED5lic7RbZlfoML8eXKivWgihqhSYKx4VYhCbrgA2VzsCMZK35R/9ryBNC\nunjg6BewazL6emLvDoIOeM/msqbeI4DF0Zdj4LA7mkOfivyG3YFQppZzlqOdPHaB\nfslA+NEkbpyv+oq6q4XAT6H7j4b/3LvfJJn5/kJVsQKBgQD6xVirgBQYhw73fROs\nzT6ROIniS+yNQsyaVzJLT37ASnIAA8XSY8Zqv9rBoanF9m7pFG8hzou5mpmrDJeD\ntucAtDAkCt4IJYHzhRcRB42YzEsz4FtSmRLcnfztV4HeRQ+qVScxAQftSWBlI3wG\ngcYJboEYgcipgIWDkt1jNlsDvQKBgQC6DBC8JxsrbX0wkLfBI9DUPmJ0Hj/ICIRj\nsaDZqGXGzTReKmsrVCHjBXwQv2i0vlb/XJTHNiaq+qi+ke1Sl1vZ7eNA7QYCSaSw\n85g1f3TwMuNL4tNYnoeov75/MW+OOMDdiWDa2su3/gGzbCyGxnu183lOMGRq25Nr\n53RYyjWv6wKBgCUqdG7Oh6vu8F/rsKvOq3Q4be8VE5UIDmzdeX3B5WY4YlbwODRr\nRy/xCUnjXNcTI/L1bVcUVo8Gg7nfHuGNjQUEr5HX7npEnNLrqbSDaw/3Tzn8CWzf\ndPUvkRnX+05m6+BxyGIx5evcESZMMQiB2UjykL2CFqxQePk8HgWjO6/9AoGBAIfB\n8hw5tPu1g1qC33NR/TpC43iz8fq3LFPK9AKffNTaPFJSRa1yUN/lyqg14+Ov2K+y\nEOZkv0aYbbXRqglQZTfa/K8Nxgq5TjHJLtIq1VvI5k8jjvDljTw33nvVyP9Qrnt5\nmXFZorn54afZUYxvHSqiJWxGp5/J5gMg+6cmtpJNAoGBALwooUf/dl5Yg/NT9Zj6\nd6krcQOJ1Iqe2O/lsDCeaUf4bfYXMldUaqLqp7MpYGW0NIHd2ke1b0QAodqTaH80\nieNImbbcaBSEU67EsAhnPBbqeQRjtzKLYOnj1OKCBL5czat5vNJ+N/kMT0jBskCq\nuM2QugS0bDLTwoUeCqNySdFf\n-----END PRIVATE KEY-----\n";
    const char DEVICE_TOKEN[] PROGMEM = "SPwa40x6RBD7s6OKe8kIiQz84lrkOlbJ8vqXmbIoewb4WL0858VSL13P32dypb9cxv7rzREW9ICSAep1Wztb195jUTsWICcsh5YCwubDjS0augq9ovDWx1QTWWz9HktGjVx4Pqp54IxOvxCVawHIV18vVPmTsa46laktz03dAOudrsCcB4tRI6PCcaO7qeyOszIGddGDQDWJCJsRdL9j9JXciIk0Z1bf8SWlLgzJDx8hbt90Y9wXWrLKSfD71mfTMs7MmYzz3HSOyIkIDr6dzdX3ajCVuBmQE84kXHkboyyG3rQ1So2Y624wGsWciFz7Ii99crj1cCtCS9tKlOZOL1w7ABkVYEYI253iSBgRH3MtAeJdcVuQW189SZ1gs27JwBuF0yL9cWq2UHy0H9A92LgfGVo4iUKcpZGkkG2UC2E5beoWzreyl9twJzll5xQ5TrspOdAPx36KzjKZMbXCGUfhBLiksRZWtkDpa47dcDKjsRr7VMGK42vrA5ql6Fxp";
}
class Firebase
{
private:
    String generateToken(uint32_t timestamp);

public:
    Firebase(/* args */);
    ~Firebase();
};

Firebase::Firebase(/* args */)
{
}

Firebase::~Firebase()
{
}

String Firebase::generateToken(uint32_t timestamp)
{
    auto minutes = timestamp / 60000;

    String input = String(FB::DEVICE_TOKEN) + ":" + String(minutes);

    // Initialize mbedTLS context
    mbedtls_md_context_t ctx;
    const mbedtls_md_info_t *info;
    mbedtls_md_init(&ctx);
    info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    if (mbedtls_md_setup(&ctx, info, 0) != 0)
    {
        mbedtls_md_free(&ctx);
        return "";
    }

    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, (const uint8_t *)input.c_str(), input.length());

    uint8_t hash[32]; // Buffer for the SHA-256 output
    mbedtls_md_finish(&ctx, hash);
    mbedtls_md_free(&ctx);

    // Convert hash to hex string
    String token = "";
    for (int i = 0; i < 32; i++)
    {
        if (hash[i] < 16)
            token += "0"; // Pad single-digit hex values
        token += String(hash[i], HEX);
    }

    return token;
}