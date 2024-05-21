module Stage = {
  type t =
    | Loading
    | Generate
    | QuickActions;

  let use = () => {
    let (stage, setStage) = React.useState(() => Loading);

    let onChange =
      React.useCallback0(generatedSecrets => {
        setStage(_ =>
          switch (generatedSecrets) {
          | SecretsHook.Loading => Loading
          | SecretsHook.NoSecrets => Generate
          | SecretsHook.HasSecrets(_) => QuickActions
          }
        )
      });

    let _ = SecretsHook.useGeneratedSecrets(onChange);

    (stage, setStage);
  };
};
