open React.Event;
open RegisterPageHooksCore;

let useField = () => {
  let (state, setState) = React.useState(() => "");
  let onChange =
    React.useCallback0((event: Form.t) => {
      Form.preventDefault(event);
      setState(_ => Form.target(event)##value);
    });
  (state, onChange);
};

let useUsername = () => {
  let (value, onChange) = useField();
  let validation =
    React.useMemo1(() => ValidationUsername.validate(value), [|value|]);
  {value, onChange, validation};
};

let usePassword = (~username: string) => {
  let (value, onChange) = useField();
  let validation =
    React.useMemo2(
      () => {ValidationPassword.validate(username, value)},
      (username, value),
    );
  {value, onChange, validation};
};

let useConfirm = (~password: string) => {
  let (value, onChange) = useField();
  let validation =
    React.useMemo2(
      () => ValidationPasswordConfirm.validate(password, value),
      (password, value),
    );
  {value, onChange, validation};
};

let useStage = (): stageHook => {
  let (stage, setStage) = React.useState(() => Form);
  {
    current: stage,
    toGenerate: _ => setStage(_ => Generate),
    toSubmit: _ => setStage(_ => Submit),
  };
};
