open Js.Typed_array;
open Promise_syntax;
open Vault_js;

type clientSecrets = {
  clientRandom: Uint8Array.t,
  derivedKey: DerivedKey.t,
  masterKeyIv: Uint8Array.t,
};

let writeClientSecrets =
    (~clientSecrets: clientSecrets): Js.Promise.t(Js.Json.t) => {
  let clientRandom = clientSecrets.clientRandom;
  let* derivedKey =
    Operations.exportDerivedKey(~derivedKey=clientSecrets.derivedKey);
  let masterKeyIv = clientSecrets.masterKeyIv;

  Js.Promise.resolve(
    Client_types_bs.write_clientSecretsCore({
      clientRandom,
      derivedKey,
      masterKeyIv,
    }),
  );
};

let readClientSecrets =
    (~clientSecretsJson: Js.Json.t): Js.Promise.t(clientSecrets) => {
  let Client_types_bs.{clientRandom, derivedKey, masterKeyIv} =
    Client_types_bs.read_clientSecretsCore(clientSecretsJson);

  let* derivedKey =
    Operations.importDerivedKey(~exportedDerivedKey=derivedKey);

  Js.Promise.resolve({clientRandom, derivedKey, masterKeyIv});
};

let writeToLocalStorage = (~clientSecrets: clientSecrets): Js.Promise.t(unit) => {
  let+ clientSecretsJson = writeClientSecrets(~clientSecrets);
  let clientSecretsString = Js.Json.stringify(clientSecretsJson);

  Dom.Storage.(localStorage |> setItem("clientSecrets", clientSecretsString));
};

let readFromLocalStorage = (): Js.Promise.t(option(clientSecrets)) => {
  let clientSecretsJson =
    Dom.Storage.(localStorage |> getItem("clientSecrets"))
    |> Option.map(Js.Json.parseExn);

  switch (clientSecretsJson) {
  | Some(clientSecretsJson) =>
    let* clientSecrets = readClientSecrets(~clientSecretsJson);
    Js.Promise.resolve(Some(clientSecrets));
  | None => Js.Promise.resolve(None)
  };
};

include Store.MakeContext({
  open Store;

  type t = option(clientSecrets);

  let use = () => {
    let subscribers = React.useRef(Subscribers_js.create());
    let get = readFromLocalStorage;
    let set = clientSecrets => {
      let+ _ =
        switch (clientSecrets) {
        | Some(clientSecrets) => writeToLocalStorage(~clientSecrets)
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
