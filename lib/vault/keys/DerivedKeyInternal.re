open Js.Typed_array;
open PromiseUtil;
open WebCrypto;

type t = cryptoKey;

type freshKey = {
  derivedKey: t,
  hashedAuthenticationKey: ArrayBuffer.t,
  masterKeyIv: Uint8Array.t,
};

let create = (~password: string, ~saltBuffer: ArrayBuffer.t) => {
  let textEncoder = TextEncoder.create();
  let encodedPassword =
    TextEncoder.encode(textEncoder, password) |> Uint8Array.buffer;
  let masterKeyIv = getRandomValues_impl(Uint8Array.fromLength(12));

  let* baseKey = {
    let format = "raw";
    let keyData = encodedPassword;
    let algorithm = "PBKDF2";
    let extractable = false;
    let keyUsages = [|"deriveBits"|];
    importKey_impl(format, keyData, algorithm, extractable, keyUsages);
  };

  let* keyBits = {
    let algorithm = {
      "name": "PBKDF2",
      "hash": "SHA-512",
      "salt": saltBuffer,
      "iterations": 100_000,
    };
    let baseKey = baseKey;
    let length = 256;
    deriveBits_impl(algorithm, baseKey, length);
  };

  let* derivedKey = {
    let derivedEncryptionKeyRaw =
      ArrayBuffer.slice(~start=0, ~end_=16, keyBits);
    let format = "raw";
    let keyData = derivedEncryptionKeyRaw;
    let algorithm = {"name": "AES-GCM"};
    let extractable = true;
    let keyUsages = [|"wrapKey", "unwrapKey"|];
    importKey_impl(format, keyData, algorithm, extractable, keyUsages);
  };

  let* hashedAuthenticationKey = {
    let derivedAuthenticationKeyRaw =
      Uint8Array.fromBuffer(~off=16, ~len=16, keyBits, ());
    digest_impl("SHA-256", derivedAuthenticationKeyRaw);
  };

  resolve({derivedKey, hashedAuthenticationKey, masterKeyIv});
};

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;
  type nonrec freshKey = freshKey;

  let create:
    (~password: string, ~saltBuffer: ArrayBuffer.t) => Js.Promise.t(freshKey);
};
