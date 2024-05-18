open WebCrypto;

type privateKey = pri cryptoKey;
type publicKey = pri cryptoKey;

type freshKeys = {
  privateKey,
  publicKey,
};

let create: unit => Js.Promise.t(freshKeys);

let privateFromCryptoKey: cryptoKey => privateKey;
let privateToCryptoKey: privateKey => cryptoKey;
let publicFromCryptoKey: cryptoKey => publicKey;
let publicToCryptoKey: publicKey => cryptoKey;
