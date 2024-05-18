open Js.Typed_array;
open Promise_syntax;
open WebCrypto;

type privateKey = cryptoKey;
type publicKey = cryptoKey;

type freshKeys = {
  privateKey,
  publicKey,
};

let create = () => {
  let* keyPair = {
    let algorithm = {
      "name": "RSA-PSS",
      "modulusLength": 4096,
      "publicExponent": Uint8Array.make([|1, 0, 1|]),
      "hash": "SHA-512",
    };
    let extractable = true;
    let keyUsages = [|"sign", "verify"|];
    generateKeyPair_impl(algorithm, extractable, keyUsages);
  };
  resolve({privateKey: keyPair.privateKey, publicKey: keyPair.publicKey});
};

let privateFromCryptoKey: cryptoKey => privateKey = Fun.id;
let privateToCryptoKey: privateKey => cryptoKey = Fun.id;
let publicFromCryptoKey: cryptoKey => publicKey = Fun.id;
let publicToCryptoKey: publicKey => cryptoKey = Fun.id;
