open Js.Typed_array;
open Promise_syntax;
open Vault_js;

type derivedSecrets = {
  saltBuffer: ArrayBuffer.t,
  derivedKey: DerivedSecrets.t,
  masterKeyIv: Uint8Array.t,
  protectionKeyIv: Uint8Array.t,
  verificationKeyIv: Uint8Array.t,
};

let writeDerivedSecrets =
    (~derivedSecrets: derivedSecrets): Js.Promise.t(Js.Json.t) => {
  let saltBuffer = derivedSecrets.saltBuffer;
  let* derivedKey =
    Operations.exportDerivedKey(~derivedKey=derivedSecrets.derivedKey);
  let masterKeyIv = derivedSecrets.masterKeyIv;
  let protectionKeyIv = derivedSecrets.protectionKeyIv;
  let verificationKeyIv = derivedSecrets.verificationKeyIv;

  Js.Promise.resolve(
    Client_types_bs.write_derivedSecretsCore({
      saltBuffer,
      derivedKey,
      masterKeyIv,
      protectionKeyIv,
      verificationKeyIv,
    }),
  );
};

let readDerivedSecrets =
    (~derivedSecretsJson: Js.Json.t): Js.Promise.t(derivedSecrets) => {
  let Client_types_bs.{
        saltBuffer,
        derivedKey,
        masterKeyIv,
        protectionKeyIv,
        verificationKeyIv,
      } =
    Client_types_bs.read_derivedSecretsCore(derivedSecretsJson);

  let* derivedKey =
    Operations.importDerivedKey(~exportedDerivedKey=derivedKey);

  Js.Promise.resolve({
    saltBuffer,
    derivedKey,
    masterKeyIv,
    protectionKeyIv,
    verificationKeyIv,
  });
};

let writeToLocalStorage =
    (~derivedSecrets: derivedSecrets): Js.Promise.t(unit) => {
  let+ derivedSecretsJson = writeDerivedSecrets(~derivedSecrets);
  let derivedSecretsString = Js.Json.stringify(derivedSecretsJson);

  Dom.Storage.(
    localStorage |> setItem("derivedSecrets", derivedSecretsString)
  );
};

let readFromLocalStorage = (): Js.Promise.t(option(derivedSecrets)) => {
  let derivedSecretsJson =
    Dom.Storage.(localStorage |> getItem("derivedSecrets"))
    |> Option.map(Js.Json.parseExn);

  switch (derivedSecretsJson) {
  | Some(derivedSecretsJson) =>
    let* derivedSecrets = readDerivedSecrets(~derivedSecretsJson);
    Js.Promise.resolve(Some(derivedSecrets));
  | None => Js.Promise.resolve(None)
  };
};

include Store.MakeContext({
  open Store;

  type t = option(derivedSecrets);

  let use = () => {
    let subscribers = React.useRef(Subscribers_js.create());
    let get = readFromLocalStorage;
    let set = derivedSecrets => {
      let+ _ =
        switch (derivedSecrets) {
        | Some(derivedSecrets) => writeToLocalStorage(~derivedSecrets)
        | None => Js.Promise.resolve()
        };
      Subscribers_js.forEach(subscribers.current, subscriber => subscriber());
    };
    let subscribe = callback => {
      let key = Subscribers_js.add(subscribers.current, callback);
      () => {
        Subscribers_js.delete(subscribers.current, key);
      };
    };
    {get, set, subscribe};
  };
});
