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
      ApiRegister.post({
        username,
        auth_token: Salt.toHash(freshDerivedKey.hashedAuthenticationKey),
        client_random: Base64_js.Uint8Array.encode(clientRandom),
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

let useLogin = () => {
  React.useCallback0((~username, ~password) => {
    let* saltResult = ApiLogin.postSalt(~username);
    switch (saltResult) {
    | Error(e) => resolve(Error(e))
    | Ok({salt}) =>
      let saltBuffer = Base64_js.ArrayBuffer.decode(salt);
      let* derivedKey = DerivedKey.create(~password, ~saltBuffer);
      let auth_token = Salt.toHash(derivedKey.hashedAuthenticationKey);

      let* authResult = ApiLogin.postAuth(~username, ~auth_token);
      switch (authResult) {
      | Error(e) => resolve(Error(e))
      | Ok(_) => resolve(Ok())
      };
    };
  });
};
