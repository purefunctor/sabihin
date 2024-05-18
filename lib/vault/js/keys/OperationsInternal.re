open Js.Typed_array;
open Promise_syntax;
open WebCrypto;

let exportDerivedKey =
    (~derivedKey: DerivedSecretsInternal.t): Js.Promise.t(ArrayBuffer.t) => {
  let format = "raw";
  exportKey_impl(format, derivedKey |> DerivedSecretsInternal.toCryptoKey);
};

let importDerivedKey =
    (~exportedDerivedKey: ArrayBuffer.t)
    : Js.Promise.t(DerivedSecretsInternal.t) => {
  let format = "raw";
  let keyData = exportedDerivedKey;
  let algorithm = {"name": "AES-GCM"};
  let extractable = true;
  let keyUsages = [|"wrapKey", "unwrapKey"|];
  let* derivedKeyRaw =
    importKey_impl(format, keyData, algorithm, extractable, keyUsages);
  resolve(derivedKeyRaw |> DerivedSecretsInternal.fromCryptoKey);
};

let wrapMasterKey =
    (
      ~derivedKey: DerivedSecretsInternal.t,
      ~masterKeyIv: Uint8Array.t,
      ~masterKey: MasterKeyInternal.t,
    ) => {
  let format = "raw";
  let key = masterKey |> MasterKeyInternal.toCryptoKey;
  let wrapping_key = derivedKey |> DerivedSecretsInternal.toCryptoKey;
  let wrapAlgo = {"name": "AES-GCM", "iv": masterKeyIv};
  wrapKey_impl(format, key, wrapping_key, wrapAlgo);
};

let unwrapMasterKey =
    (
      ~derivedKey: DerivedSecretsInternal.t,
      ~masterKeyIv: Uint8Array.t,
      ~encryptedMasterKey: ArrayBuffer.t,
    )
    : Js.Promise.t(MasterKeyInternal.t) => {
  let format = "raw";
  let wrappedKey = encryptedMasterKey;
  let unwrappingKey = derivedKey |> DerivedSecretsInternal.toCryptoKey;
  let unwrapAlgo = {"name": "AES-GCM", "iv": masterKeyIv};
  let unwrappedKeyAlgo = {"name": "AES-GCM"};
  let extractable = true;
  let keyUsages = [|"wrapKey", "unwrapKey"|];
  let* rawMasterKey =
    unwrapKey_impl(
      format,
      wrappedKey,
      unwrappingKey,
      unwrapAlgo,
      unwrappedKeyAlgo,
      extractable,
      keyUsages,
    );
  resolve(rawMasterKey |> MasterKeyInternal.fromCryptoKey);
};

let wrapProtectionPrivateKey =
    (
      ~masterKey: MasterKeyInternal.t,
      ~protectionKeyIv: Uint8Array.t,
      ~protectionPrivateKey: ProtectionKeysInternal.privateKey,
    )
    : Js.Promise.t(ArrayBuffer.t) => {
  let format = "pkcs8";
  let key = protectionPrivateKey |> ProtectionKeysInternal.privateToCryptoKey;
  let wrappingKey = masterKey |> MasterKeyInternal.toCryptoKey;
  let wrapAlgo = {"name": "AES-GCM", "iv": protectionKeyIv};
  wrapKey_impl(format, key, wrappingKey, wrapAlgo);
};

let unwrapProtectionPrivateKey =
    (
      ~masterKey: MasterKeyInternal.t,
      ~protectionKeyIv: Uint8Array.t,
      ~encryptedProtectionPrivateKey: ArrayBuffer.t,
    )
    : Js.Promise.t(ProtectionKeysInternal.privateKey) => {
  let format = "pkcs8";
  let wrappedKey = encryptedProtectionPrivateKey;
  let unwrappingKey = masterKey |> MasterKeyInternal.toCryptoKey;
  let unwrapAlgo = {"name": "AES-GCM", "iv": protectionKeyIv};
  let unwrappedKeyAlgo = {"name": "RSA-OAEP", "hash": "SHA-512"};
  let extractable = true;
  let keyUsages = [|"unwrapKey"|];
  let* privateKeyRaw =
    unwrapKey_impl(
      format,
      wrappedKey,
      unwrappingKey,
      unwrapAlgo,
      unwrappedKeyAlgo,
      extractable,
      keyUsages,
    );
  resolve(privateKeyRaw |> ProtectionKeysInternal.privateFromCryptoKey);
};

let exportProtectionPublicKey =
    (~protectionPublicKey: ProtectionKeysInternal.publicKey)
    : Js.Promise.t(ArrayBuffer.t) => {
  let format = "spki";
  exportKey_impl(
    format,
    protectionPublicKey |> ProtectionKeysInternal.publicToCryptoKey,
  );
};

let wrapVerificationPrivateKey =
    (
      ~masterKey: MasterKeyInternal.t,
      ~verificationKeyIv: Uint8Array.t,
      ~verificationPrivateKey: VerificationKeysInternal.privateKey,
    )
    : Js.Promise.t(ArrayBuffer.t) => {
  let format = "pkcs8";
  let key =
    verificationPrivateKey |> VerificationKeysInternal.privateToCryptoKey;
  let wrappingKey = masterKey |> MasterKeyInternal.toCryptoKey;
  let wrapAlgo = {"name": "AES-GCM", "iv": verificationKeyIv};
  wrapKey_impl(format, key, wrappingKey, wrapAlgo);
};

let unwrapVerificationPrivateKey =
    (
      ~masterKey: MasterKeyInternal.t,
      ~verificationKeyIv: Uint8Array.t,
      ~encryptedVerificationPrivateKey: ArrayBuffer.t,
    )
    : Js.Promise.t(VerificationKeysInternal.privateKey) => {
  let format = "pkcs8";
  let wrappedKey = encryptedVerificationPrivateKey;
  let unwrappingKey = masterKey |> MasterKeyInternal.toCryptoKey;
  let unwrapAlgo = {"name": "AES-GCM", "iv": verificationKeyIv};
  let unwrappedKeyAlgo = {"name": "RSA-OAEP", "hash": "SHA-512"};
  let extractable = true;
  let keyUsages = [|"unwrapKey"|];
  let* privateKeyRaw =
    unwrapKey_impl(
      format,
      wrappedKey,
      unwrappingKey,
      unwrapAlgo,
      unwrappedKeyAlgo,
      extractable,
      keyUsages,
    );
  resolve(privateKeyRaw |> VerificationKeysInternal.privateFromCryptoKey);
};

let exportVerificationPublicKey =
    (~verificationPublicKey: VerificationKeysInternal.publicKey)
    : Js.Promise.t(ArrayBuffer.t) => {
  let format = "spki";
  exportKey_impl(
    format,
    verificationPublicKey |> VerificationKeysInternal.publicToCryptoKey,
  );
};

let wrapEphemeralKey =
    (
      ~protectionPublicKey: ProtectionKeysInternal.publicKey,
      ~ephemeralKey: EphemeralKeyInternal.t,
    )
    : Js.Promise.t(ArrayBuffer.t) => {
  let format = "raw";
  let key = ephemeralKey |> EphemeralKeyInternal.toCryptoKey;
  let wrappingKey =
    protectionPublicKey |> ProtectionKeysInternal.publicToCryptoKey;
  let wrapAlgo = {"name": "RSA-OAEP"};
  wrapKey_impl(format, key, wrappingKey, wrapAlgo);
};

let unwrapEphemeralKey =
    (
      ~protectionPrivateKey: ProtectionKeysInternal.privateKey,
      ~encryptedEphemeralKey: ArrayBuffer.t,
    )
    : Js.Promise.t(EphemeralKeyInternal.t) => {
  let format = "raw";
  let wrappedKey = encryptedEphemeralKey;
  let unwrappingKey =
    protectionPrivateKey |> ProtectionKeysInternal.privateToCryptoKey;
  let unwrapAlgo = {"name": "RSA-OAEP"};
  let unwrappedKeyAlgo = {"name": "AES-GCM"};
  let extractable = true;
  let keyUsages = [|"encrypt", "decrypt"|];
  let* ephemeralKeyRaw =
    unwrapKey_impl(
      format,
      wrappedKey,
      unwrappingKey,
      unwrapAlgo,
      unwrappedKeyAlgo,
      extractable,
      keyUsages,
    );
  resolve(ephemeralKeyRaw |> EphemeralKeyInternal.fromCryptoKey);
};

let signData =
    (
      ~verificationPrivateKey: VerificationKeysInternal.privateKey,
      data: ArrayBuffer.t,
    )
    : Js.Promise.t(ArrayBuffer.t) => {
  let algorithm = {"name": "RSA-PSS", "saltLength": 64};
  sign_impl(
    algorithm,
    verificationPrivateKey |> VerificationKeysInternal.privateToCryptoKey,
    data,
  );
};

let verifyData =
    (
      ~verificationPublicKey: VerificationKeysInternal.publicKey,
      ~signature: ArrayBuffer.t,
      data: ArrayBuffer.t,
    )
    : Js.Promise.t(bool) => {
  let algorithm = {"name": "RSA-PSS", "saltLength": 64};
  verify_impl(
    algorithm,
    verificationPublicKey |> VerificationKeysInternal.publicToCryptoKey,
    signature,
    data,
  );
};

let encryptData =
    (
      ~ephemeralKey: EphemeralKeyInternal.t,
      ~messageIv: Uint8Array.t,
      data: ArrayBuffer.t,
    )
    : Js.Promise.t(ArrayBuffer.t) => {
  let algorithm = {"name": "AES-GCM", "iv": messageIv};
  let key = ephemeralKey |> EphemeralKeyInternal.toCryptoKey;
  encrypt_impl(algorithm, key, data);
};

let decryptData =
    (
      ~ephemeralKey: EphemeralKeyInternal.t,
      ~messageIv: Uint8Array.t,
      data: ArrayBuffer.t,
    )
    : Js.Promise.t(ArrayBuffer.t) => {
  let algorithm = {"name": "AES-GCM", "iv": messageIv};
  let key = ephemeralKey |> EphemeralKeyInternal.toCryptoKey;
  decrypt_impl(algorithm, key, data);
};
