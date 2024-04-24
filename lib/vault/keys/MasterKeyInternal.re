open Js.Typed_array;
open PromiseUtil;
open WebCrypto;

type t = cryptoKey;
type protectionKeyIv = Uint8Array.t;
type verificationKeyIv = Uint8Array.t;

type freshKey = {
  masterKey: t,
  protectionKeyIv,
  verificationKeyIv,
};

let create = (~saltBuffer: ArrayBuffer.t): Js.Promise.t(freshKey) => {
  let randomData =
    getRandomValues_impl(Uint8Array.fromLength(128)) |> Uint8Array.buffer;
  let protectionKeyIv = getRandomValues_impl(Uint8Array.fromLength(12));
  let verificationKeyIv = getRandomValues_impl(Uint8Array.fromLength(12));

  let* baseKey = {
    let format = "raw";
    let keyData = randomData;
    let algorithm = "HKDF";
    let extractable = false;
    let keyUsages = [|"deriveKey"|];
    importKey_impl(format, keyData, algorithm, extractable, keyUsages);
  };

  let* masterKey = {
    let algoritm = {
      "name": "HKDF",
      "hash": "SHA-512",
      "salt": saltBuffer,
      "info": Uint8Array.make([||]),
    };
    let baseKey = baseKey;
    let derivedKeyAlgorithm = {"name": "AES-GCM", "length": 256};
    let extractable = true;
    let keyUsages = [|"wrapKey", "unwrapKey"|];
    deriveKey_impl(
      algoritm,
      baseKey,
      derivedKeyAlgorithm,
      extractable,
      keyUsages,
    );
  };

  resolve({masterKey, protectionKeyIv, verificationKeyIv});
};

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;
  type nonrec protectionKeyIv = protectionKeyIv;
  type nonrec verificationKeyIv = verificationKeyIv;
  type nonrec freshKey = freshKey;

  let create: (~saltBuffer: ArrayBuffer.t) => Js.Promise.t(freshKey);
};
