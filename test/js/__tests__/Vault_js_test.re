open Jest;
open Expect;
open Js.Typed_array;
open Promise_syntax;
open Vault_js;

describe("Salt", () => {
  test("it works", () => {
    let text_encoder = TextEncoder.create();
    let text_decoder = TextDecoder.create();

    let encoded_text = TextEncoder.encode(text_encoder, "ABCDEF0123456789");
    let salt_base = Salt.computeBase(encoded_text);
    let decoded_salt = TextDecoder.decode(text_decoder, salt_base);

    assert(Uint8Array.length(salt_base) == 256);

    expect(decoded_salt)
    |> toBe(
         "sabihin.phLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLABCDEF0123456789",
       );
  })
});

describe("Derived Key", () => {
  testPromise("it works", () => {
    let clientRandom = ClientRandom.create();
    let* saltBuffer = Salt.computeDigest(clientRandom);
    let* freshMasterKey = MasterKey.create(~saltBuffer);
    let* freshDerivedKey =
      DerivedKey.create(~password="Maho_Akashi_9_20", ~saltBuffer);

    let* encryptedMasterKey =
      Operations.wrapMasterKey(
        ~derivedKey=freshDerivedKey.derivedKey,
        ~masterKeyIv=freshDerivedKey.masterKeyIv,
        ~masterKey=freshMasterKey.masterKey,
      );

    let* _ =
      Operations.unwrapMasterKey(
        ~derivedKey=freshDerivedKey.derivedKey,
        ~masterKeyIv=freshDerivedKey.masterKeyIv,
        ~encryptedMasterKey,
      );

    resolve(pass);
  });

  testPromise("it can be exported and imported", () => {
    let clientRandom = ClientRandom.create();
    let* saltBuffer = Salt.computeDigest(clientRandom);
    let* freshDerivedKey =
      DerivedKey.create(~password="Maho_Akashi_9_20", ~saltBuffer);

    let* exportedDerivedKey =
      Operations.exportDerivedKey(~derivedKey=freshDerivedKey.derivedKey);

    let* _ = Operations.importDerivedKey(~exportedDerivedKey);

    resolve(pass);
  });
});

describe("Master Key", () => {
  testPromise("it wraps and unwraps a protection private key", () => {
    let clientRandom = ClientRandom.create();
    let* saltBuffer = Salt.computeDigest(clientRandom);
    let* freshMasterKey = MasterKey.create(~saltBuffer);
    let* protectionKeyPair = ProtectionKeys.create();
    let* encryptedProtectionPrivateKey =
      Operations.wrapProtectionPrivateKey(
        ~masterKey=freshMasterKey.masterKey,
        ~protectionKeyIv=freshMasterKey.protectionKeyIv,
        ~protectionPrivateKey=protectionKeyPair.privateKey,
      );
    let* _ =
      Operations.unwrapProtectionPrivateKey(
        ~masterKey=freshMasterKey.masterKey,
        ~protectionKeyIv=freshMasterKey.protectionKeyIv,
        ~encryptedProtectionPrivateKey,
      );
    resolve(pass);
  });

  testPromise("it wraps and unwraps a verification private key", () => {
    let clientRandom = ClientRandom.create();
    let* saltBuffer = Salt.computeDigest(clientRandom);
    let* freshMasterKey = MasterKey.create(~saltBuffer);
    let* verificationKeyPair = VerificationKeys.create();
    let* encryptedVerificationPrivateKey =
      Operations.wrapVerificationPrivateKey(
        ~masterKey=freshMasterKey.masterKey,
        ~verificationKeyIv=freshMasterKey.verificationKeyIv,
        ~verificationPrivateKey=verificationKeyPair.privateKey,
      );
    let* _ =
      Operations.unwrapVerificationPrivateKey(
        ~masterKey=freshMasterKey.masterKey,
        ~verificationKeyIv=freshMasterKey.verificationKeyIv,
        ~encryptedVerificationPrivateKey,
      );
    resolve(pass);
  });
});

describe("Protection Key Pair", () => {
  testPromise("it wraps and unwraps an ephemeral key", () => {
    let* protectionKeys = ProtectionKeys.create();
    let* freshEphemeralKey = EphemeralKey.create();
    let* encryptedEphemeralKey =
      Operations.wrapEphemeralKey(
        ~protectionPublicKey=protectionKeys.publicKey,
        ~ephemeralKey=freshEphemeralKey.ephemeralKey,
      );
    let* _ =
      Operations.unwrapEphemeralKey(
        ~protectionPrivateKey=protectionKeys.privateKey,
        ~encryptedEphemeralKey,
      );
    resolve(pass);
  })
});

describe("Verification Key Pair", () => {
  testPromise("it signs and verifies a message", () => {
    let textEncoder = TextEncoder.create();

    let message = {js|ハッピーアラウンド！|js};
    let data = Uint8Array.buffer(TextEncoder.encode(textEncoder, message));

    let* verificationKeys = VerificationKeys.create();
    let* signature =
      Operations.signData(
        ~verificationPrivateKey=verificationKeys.privateKey,
        data,
      );

    let* success =
      Operations.verifyData(
        ~verificationPublicKey=verificationKeys.publicKey,
        ~signature,
        data,
      );

    resolve(expect(success) |> toBe(true));
  })
});

describe("Ephemeral Key", () => {
  testPromise("it encrypts and decrypts a message", () => {
    let text_encoder = TextEncoder.create();
    let text_decoder = TextDecoder.create();

    let message = {js|ハッピーアラウンド！|js};
    let encodedMessage =
      Uint8Array.buffer(TextEncoder.encode(text_encoder, message));

    let* freshEphemeralKey = EphemeralKey.create();
    let* encryptedData =
      Operations.encryptData(
        ~ephemeralKey=freshEphemeralKey.ephemeralKey,
        ~messageIv=freshEphemeralKey.messageIv,
        encodedMessage,
      );
    let* decryptedData =
      Operations.decryptData(
        ~ephemeralKey=freshEphemeralKey.ephemeralKey,
        ~messageIv=freshEphemeralKey.messageIv,
        encryptedData,
      );
    let decryptedData = Uint8Array.fromBuffer(decryptedData, ());
    let decryptedMessage = TextDecoder.decode(text_decoder, decryptedData);
    resolve(expect(message) |> toBe(decryptedMessage));
  })
});
