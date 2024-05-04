open Js.Typed_array;
open Promise_syntax;
open WebCrypto;

type t = cryptoKey;

type derivedSecrets = {
  derivedKey: t,
  authenticationKey: string,
  masterKeyIv: Uint8Array.t,
  protectionKeyIv: Uint8Array.t,
  verificationKeyIv: Uint8Array.t,
};

let create =
    (~password: string, ~saltBuffer: ArrayBuffer.t)
    : Js.Promise.t(derivedSecrets) => {
  let textEncoder = TextEncoder.create();
  let encodedPassword =
    TextEncoder.encode(textEncoder, password) |> Uint8Array.buffer;

  let* baseKey = {
    let format = "raw";
    let keyData = encodedPassword;
    let algorithm = "PBKDF2";
    let extractable = false;
    let keyUsages = [|"deriveBits"|];
    importKey_impl(format, keyData, algorithm, extractable, keyUsages);
  };

  // 128 bits: derivedKey
  // 128 bits: authenticationKey
  //  96 bits: masterKeyIv
  //  96 bits: protectionKeyIv
  //  96 bits: verificationKeyIv
  let* keyBits = {
    let algorithm = {
      "name": "PBKDF2",
      "hash": "SHA-512",
      "salt": saltBuffer,
      "iterations": 100_000,
    };
    let baseKey = baseKey;
    let length = 128 + 128 + 96 + 96 + 96;
    deriveBits_impl(algorithm, baseKey, length);
  };

  let derivedKeyRaw = ArrayBuffer.slice(~start=0, ~end_=16, keyBits);
  let authenticationKeyRaw =
    Uint8Array.fromBuffer(~off=16, ~len=16, keyBits, ());
  let masterKeyIv = Uint8Array.fromBuffer(~off=32, ~len=12, keyBits, ());
  let protectionKeyIv = Uint8Array.fromBuffer(~off=44, ~len=12, keyBits, ());
  let verificationKeyIv =
    Uint8Array.fromBuffer(~off=56, ~len=12, keyBits, ());

  let* derivedKey = {
    let format = "raw";
    let keyData = derivedKeyRaw;
    let algorithm = {"name": "AES-GCM"};
    let extractable = true;
    let keyUsages = [|"wrapKey", "unwrapKey"|];
    importKey_impl(format, keyData, algorithm, extractable, keyUsages);
  };

  let* authenticationKey = {
    let+ authenticationKeyDigest =
      digest_impl("SHA-256", authenticationKeyRaw);
    Salt.toHash(authenticationKeyDigest);
  };

  resolve({
    derivedKey,
    authenticationKey,
    masterKeyIv,
    protectionKeyIv,
    verificationKeyIv,
  });
};

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;
  type nonrec derivedSecrets = derivedSecrets;

  let create:
    (~password: string, ~saltBuffer: ArrayBuffer.t) =>
    Js.Promise.t(derivedSecrets);
};
