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
let usePassword = useNothing;
let useFormSubmit = (~username as _, ~password as _, _) => ();
