open Js.Typed_array;
open Promise_syntax;

type generatedSecrets = {
  encryptedMasterKey: ArrayBuffer.t,
  encryptedProtectionKey: ArrayBuffer.t,
  exportedProtectionKey: ArrayBuffer.t,
  encryptedVerificationKey: ArrayBuffer.t,
  exportedVerificationKey: ArrayBuffer.t,
};

let writeGeneratedSecrets =
    (~generatedSecrets: generatedSecrets): Js.Promise.t(Js.Json.t) => {
  let encryptedMasterKey = generatedSecrets.encryptedMasterKey;
  let encryptedProtectionKey = generatedSecrets.encryptedProtectionKey;
  let exportedProtectionKey = generatedSecrets.exportedProtectionKey;
  let encryptedVerificationKey = generatedSecrets.encryptedVerificationKey;
  let exportedVerificationKey = generatedSecrets.exportedVerificationKey;

  Js.Promise.resolve(
    Client_types_bs.write_generatedSecretsCore({
      encryptedMasterKey,
      encryptedProtectionKey,
      exportedProtectionKey,
      encryptedVerificationKey,
      exportedVerificationKey,
    }),
  );
};

let readGeneratedSecrets =
    (~generatedSecretsJson: Js.Json.t): Js.Promise.t(generatedSecrets) => {
  let Client_types_bs.{
        encryptedMasterKey,
        encryptedProtectionKey,
        exportedProtectionKey,
        encryptedVerificationKey,
        exportedVerificationKey,
      } =
    Client_types_bs.read_generatedSecretsCore(generatedSecretsJson);

  Js.Promise.resolve({
    encryptedMasterKey,
    encryptedProtectionKey,
    exportedProtectionKey,
    encryptedVerificationKey,
    exportedVerificationKey,
  });
};

let writeToLocalStorage =
    (~generatedSecrets: generatedSecrets): Js.Promise.t(unit) => {
  let+ generatedSecretsJson = writeGeneratedSecrets(~generatedSecrets);
  let generatedSecretsString = Js.Json.stringify(generatedSecretsJson);

  Dom.Storage.(
    localStorage |> setItem("generatedSecrets", generatedSecretsString)
  );
};

let readFromLocalStorage = (): Js.Promise.t(option(generatedSecrets)) => {
  let generatedSecretsJson =
    Dom.Storage.(localStorage |> getItem("generatedSecrets"))
    |> Option.map(Js.Json.parseExn);

  switch (generatedSecretsJson) {
  | Some(generatedSecretsJson) =>
    let* generatedSecrets = readGeneratedSecrets(~generatedSecretsJson);
    Js.Promise.resolve(Some(generatedSecrets));
  | None => Js.Promise.resolve(None)
  };
};

include Store.MakeContext({
  open Store;

  type t = option(generatedSecrets);

  let use = () => {
    let subscribers = React.useRef(Subscribers_js.create());
    let get = readFromLocalStorage;
    let set = generatedSecrets => {
      let+ _ =
        switch (generatedSecrets) {
        | Some(generatedSecrets) => writeToLocalStorage(~generatedSecrets)
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
