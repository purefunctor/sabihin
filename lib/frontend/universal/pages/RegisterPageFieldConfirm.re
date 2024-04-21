open RegisterPageFieldCore;

let hintFn =
    (validation: ValidationResult.t(ValidationCore.PasswordConfirm.t)) => {
  switch (validation) {
  | Validated(validated) =>
    let (className, text) =
      switch (validated) {
      | Yes => (successCss, "Passwords match. You're good to go!")
      | No => (failureCss, "Passwords do not match.")
      };
    <span className> {React.string(text)} </span>;
  | NotValidated => React.null
  };
};

let fieldCssFn =
    (validation: ValidationResult.t(ValidationCore.PasswordConfirm.t)) => {
  switch (validation) {
  | Validated(validated) =>
    switch (validated) {
    | Yes => " " ++ fieldSuccessCss
    | No => " " ++ fieldFailureCss
    }
  | NotValidated => ""
  };
};

include MakeField({
  type kind = ValidationCore.PasswordConfirm.t;

  let meta = {
    title: "Confirm Password",
    name: "confirm",
    type_: "password",
    placeholder: "confirm password (remember it!)",
  };
  let iconFn = _ => <Icons.LockPasswordLine size="1.5rem" />;
  let hintFn = hintFn;
  let fieldCssFn = fieldCssFn;
});
