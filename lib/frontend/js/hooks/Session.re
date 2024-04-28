open Promise_syntax;
open Vault_js;

let useSession = () => {
  let store = SessionContext.useContext();
  let (kind, setKind) = React.useState(() => `Guest);

  React.useEffect0(() => {
    let _ = {
      let+ current = store.get();
      setKind(_ => current);
    };

    Some(
      store.subscribe(() => {
        let _ = {
          let+ current = store.get();
          setKind(_ => current);
        };
        ();
      }),
    );
  });

  kind;
};

let useRegister = () => {
  let sessionStore = SessionContext.useContext();
  let clientSecretsStore = ClientSecretsContext.useContext();

  React.useCallback0((~username, ~password) => {
    let clientRandom = ClientRandom.create();
    let* saltBuffer = Salt.computeDigest(clientRandom);
    let* freshDerivedKey = DerivedKey.create(~password, ~saltBuffer);

    let* registerResult =
      ApiRegister.register({
        username,
        auth_token: Salt.toHash(freshDerivedKey.hashedAuthenticationKey),
      });

    let* _ =
      switch (registerResult) {
      | Ok(registerResult) =>
        let* _ =
          sessionStore.set(`LoggedIn({public_id: registerResult.public_id}));
        clientSecretsStore.set(
          Some({
            clientRandom,
            derivedKey: freshDerivedKey.derivedKey,
            masterKeyIv: freshDerivedKey.masterKeyIv,
          }),
        );
      | Error(_) => Js.Promise.resolve()
      };

    Js.Promise.resolve(registerResult);
  });
};
