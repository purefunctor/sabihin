open Js.Typed_array;
open Promise_syntax;
open WebCrypto;

type t = cryptoKey;

let create = (~saltBuffer: ArrayBuffer.t): Js.Promise.t(t) => {
  let randomData =
    getRandomValues_impl(Uint8Array.fromLength(128)) |> Uint8Array.buffer;

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

  resolve(masterKey);
};

let fromCryptoKey: cryptoKey => t = Fun.id;
let toCryptoKey: t => cryptoKey = Fun.id;
