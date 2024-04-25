open Vault_js;

let useSession = () => {
  let store = SessionContext.useContext();
  let (kind, setKind) = React.useState(() => `Guest);

  React.useEffect0(() => {
    setKind(_ => store.get());
    Some(store.subscribe(() => setKind(_ => store.get())));
  });

  kind;
};

let useRegister = () => {
  let store = SessionContext.useContext();
  React.useCallback0((~username, ~password) => {
    let ( let* ) = (f, x) => Js.Promise.then_(x, f);

    let clientRandom = ClientRandom.create();
    let* saltBuffer = Salt.computeDigest(clientRandom);
    let* freshDerivedKey = DerivedKey.create(~password, ~saltBuffer);

    let* registerResult =
      ApiRegister.register({
        username,
        auth_token: Salt.toHash(freshDerivedKey.hashedAuthenticationKey),
      });

    switch (registerResult) {
    | Ok(registerResult) =>
      store.set(`LoggedIn({public_id: registerResult.public_id}))
    | Error(_) => ()
    };

    Js.Promise.resolve(registerResult);
  });
};
