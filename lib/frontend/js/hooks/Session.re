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
    let* derivedSecrets = DerivedSecrets.create(~password, ~saltBuffer);

    let* registerResult =
      ApiRegister.post({
        username,
        auth_token: derivedSecrets.authenticationKey,
        client_random: Base64_js.Uint8Array.encode(clientRandom),
      });

    switch (registerResult) {
    | Error(e) => resolve(Error(e))
    | Ok(registerResult) =>
      let* _ =
        sessionStore.set(`LoggedIn({public_id: registerResult.public_id}));
      let* _ =
        clientSecretsStore.set(
          Some({
            saltBuffer,
            derivedKey: derivedSecrets.derivedKey,
            masterKeyIv: derivedSecrets.masterKeyIv,
            protectionKeyIv: derivedSecrets.protectionKeyIv,
            verificationKeyIv: derivedSecrets.verificationKeyIv,
          }),
        );
      resolve(Ok(registerResult));
    };
  });
};

let useLogin = () => {
  let sessionStore = SessionContext.useContext();
  let clientSecretsStore = ClientSecretsContext.useContext();

  React.useCallback0((~username, ~password) => {
    let* saltResult = ApiLogin.postSalt(~username);
    switch (saltResult) {
    | Error(e) => resolve(Error(e))
    | Ok({salt}) =>
      let saltBuffer = Base64_js.ArrayBuffer.decode(salt);
      let* derivedSecrets = DerivedSecrets.create(~password, ~saltBuffer);
      let auth_token = derivedSecrets.authenticationKey;

      let* authResult = ApiLogin.postAuth(~username, ~auth_token);
      switch (authResult) {
      | Error(e) => resolve(Error(e))
      | Ok(authResult) =>
        let* _ =
          sessionStore.set(`LoggedIn({public_id: authResult.public_id}));
        let* _ =
          clientSecretsStore.set(
            Some({
              saltBuffer,
              derivedKey: derivedSecrets.derivedKey,
              masterKeyIv: derivedSecrets.masterKeyIv,
              protectionKeyIv: derivedSecrets.protectionKeyIv,
              verificationKeyIv: derivedSecrets.verificationKeyIv,
            }),
          );
        resolve(Ok(authResult));
      };
    };
  });
};
