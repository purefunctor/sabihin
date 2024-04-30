open RegisterPageFieldCore;

let hintFn = (validated: ValidationResult.t(ValidationCore.PasswordBasic.t)) => {
  switch (validated) {
  | Validated(validated) =>
    let (className, text) =
      switch (validated) {
      | TooShort => (failureCss, "Password must be at least 8 characters.")
      | Success => (successCss, "Awesome, you're good to go.")
      };
    <span className> {React.string(text)} </span>;
  | NotValidated => React.null
  };
};

let fieldCssFn =
    (validated: ValidationResult.t(ValidationCore.PasswordBasic.t)) => {
  switch (validated) {
  | Validated(validated) =>
    switch (validated) {
    | TooShort => " " ++ fieldFailureCss
    | Success => " " ++ fieldSuccessCss
    }
  | NotValidated => ""
  };
};

include MakeField({
  type kind = ValidationCore.PasswordBasic.t;

  let meta = {
    title: "Password",
    name: "password",
    type_: "password",
    placeholder: "password (min. 8 characters)",
  };
  let iconFn = _ => <Icons.LockPasswordLine size="1.5rem" />;
  let hintFn = hintFn;
  let fieldCssFn = fieldCssFn;
});
