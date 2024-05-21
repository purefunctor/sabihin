open Promise_syntax;

type generatedSecretsState =
  | Loading
  | NoSecrets
  | HasSecrets(GeneratedSecretsContext.generatedSecrets);

let useGeneratedSecrets = onChange => {
  let store = GeneratedSecretsContext.useContext();
  let (state, setState) = React.useState(() => Loading);

  React.useEffect0(() => {
    let f = () => {
      let _ = {
        let* fromLocal = store.get();
        let+ localOrServer =
          switch (fromLocal) {
          | Some(_) => resolve(fromLocal)
          | None =>
            let* fromServer = GeneratedSecretsContext.requestFromServer();
            let+ _ = store.set(fromServer);
            fromServer;
          };
        let nextState =
          switch (localOrServer) {
          | None => NoSecrets
          | Some(localOrServer) => HasSecrets(localOrServer)
          };
        setState(_ => nextState);
        onChange(nextState);
      };
      ();
    };
    let timeoutId = Js.Global.setTimeout(~f, 500);
    Some(() => Js.Global.clearTimeout(timeoutId));
  });

  state;
};
