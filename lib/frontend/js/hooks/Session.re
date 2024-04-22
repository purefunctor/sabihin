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
  React.useCallback0(payload => {
    open Js.Promise;
    let ( let* ) = (f, x) => then_(x, f);

    let* registerResult = ApiRegister.register(payload);
    switch (registerResult) {
    | Ok(registerResult) =>
      store.set(`LoggedIn({public_id: registerResult.public_id}))
    | Error(_) => ()
    };

    Js.Promise.resolve(registerResult);
  });
};
