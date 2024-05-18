open Promise_syntax;

module Stage = {
  type t =
    | Loading
    | Guest
    | LoggedIn;

  let use = () => {
    let (state, setState) = React.useState(() => Loading);
    let sessionStore = SessionContext.useContext();

    React.useEffect0(() => {
      let f = () => {
        let _ = {
          let+ sessionKind = sessionStore.get();
          setState(_ =>
            switch (sessionKind) {
            | `Guest => Guest
            | `LoggedIn(_) => LoggedIn
            }
          );
        };
        ();
      };
      let timeoutId = Js.Global.setTimeout(~f, 500);
      let unsubscribe = sessionStore.subscribe(f);
      Some(
        () => {
          Js.Global.clearTimeout(timeoutId);
          unsubscribe();
        },
      );
    });

    (state, setState);
  };
};
