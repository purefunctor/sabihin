open Jest;
open Expect;
open Promise_syntax;
open Vault_js;

describe("Base64Utils", () => {
  testPromise("it works round trip", () => {
    let clientRandom = ClientRandom.create();
    let* saltBuffer = Salt.computeDigest(clientRandom);
    let* masterKey = MasterKey.create(~saltBuffer);
    let* derivedSecrets =
      DerivedSecrets.create(~password="password", ~saltBuffer);
    let* wrappedMasterKey =
      Operations.wrapMasterKey(
        ~derivedKey=derivedSecrets.derivedKey,
        ~masterKeyIv=derivedSecrets.masterKeyIv,
        ~masterKey,
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
