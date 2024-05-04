open Promise_syntax;

module Stage = {
  type t =
    | Loading
    | Generate
    | QuickActions;

  let use = () => {
    let (stage, setStage) = React.useState(() => Loading);
    let generatedSecretsStore = GeneratedSecretsContext.useContext();

    React.useEffect0(() => {
      let f = () => {
        let _ = {
          let* fromLocal = generatedSecretsStore.get();
          switch (fromLocal) {
          | Some(_) =>
            setStage(_ => QuickActions);
            resolve();
          | None =>
            let* fromServer = GeneratedSecretsContext.requestFromServer();
            let+ _ = generatedSecretsStore.set(fromServer);
            if (Option.is_some(fromServer)) {
              setStage(_ => QuickActions);
            } else {
              setStage(_ => Generate);
            };
          };
        };
        ();
      };
      let timeoutId = Js.Global.setTimeout(~f, 500);
      Some(() => {Js.Global.clearTimeout(timeoutId)});
    });

    (stage, setStage);
  };
};
