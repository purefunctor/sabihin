open Jest;
open Expect;
open Vault_js;

let resolve = Js.Promise.resolve;
let ( let* ) = (f, x) => Js.Promise.then_(x, f);

describe("Base64Utils", () => {
  testPromise("it works round trip", () => {
    let clientRandom = ClientRandom.create();
    let* saltBuffer = Salt.computeDigest(clientRandom);
    let* freshMasterKey = MasterKey.create(~saltBuffer);
    let* freshDerivedKey =
      DerivedKey.create(~password="password", ~saltBuffer);
    let* wrappedMasterKey =
      Operations.wrapMasterKey(
        ~derivedKey=freshDerivedKey.derivedKey,
        ~masterKeyIv=freshDerivedKey.masterKeyIv,
        ~masterKey=freshMasterKey.masterKey,
      );

    let base64 = Base64_js.ArrayBuffer.encode(wrappedMasterKey);
    let length = String.length(base64);

    let base64' =
      base64 |> Base64_js.ArrayBuffer.decode |> Base64_js.ArrayBuffer.encode;
    let length' = String.length(base64');

    resolve(
      expect((base64, length > 0, length == length'))
      |> toEqual((base64', true, true)),
    );
  })
});
