open RegisterPageHooksCore;

let useNothing = () => {
  {value: "", onChange: _ => (), validation: NotValidated};
};

let useUsername = useNothing;

let usePassword = (~username: string) => {
  let _ = username;
  useNothing();
};

let useConfirm = (~password: string) => {
  let _ = password;
  useNothing();
};

let useFormSubmit = (~username, ~password, ~confirm) => {
  let _ = (username, password, confirm);
  _ => ();
};

let useStage = (): stageHook => {
  {current: Form, toGenerate: _ => (), toSubmit: _ => ()};
};
