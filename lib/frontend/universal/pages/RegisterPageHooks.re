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

let useFormSubmit = (~username, ~password, ~confirm, ~toGenerate) => {
  let _ = (username, password, confirm, toGenerate);
  _ => ();
};

let useStage = () => {
  {current: Form, toGenerate: _ => (), toSubmit: _ => ()};
};
