open RegisterPageHooksCore;

let useNothing = () => {
  {
    value: "",
    onChange: _ => (),
    validation: NotValidated,
    setValidation: _ => (),
  };
};

let useUsername = useNothing;

let usePassword = (~username as _: string) => {
  useNothing();
};

let useConfirm = (~password as _: string) => {
  useNothing();
};

let useFormSubmit = (~username as _, ~password as _, ~confirm as _, _) => ();
