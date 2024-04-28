module Stage = {
  type t =
    | Loading
    | Generate
    | QuickActions;

  let use = () => {
    let (stage, setStage) = React.useState(() => Loading);

    React.useEffect0(() => {
      let f = () => setStage(_ => Generate);
      let timeoutId = Js.Global.setTimeout(~f, 500);
      Some(() => {Js.Global.clearTimeout(timeoutId)});
    });

    (stage, setStage);
  };
};
