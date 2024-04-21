open RegisterPageFieldCore;

let hintFn = (validated: ValidationResult.t(ValidationCore.Password.t)) => {
  switch (validated) {
  | Validated(validated) =>
    let (className, text) =
      switch (validated) {
      | TooShort => (failureCss, "Password must be at least 8 characters.")
      | TooWeak => (failureCss, "Password is too weak.")
      | VeryWeak => (failureCss, "Password is very weak.")
      | Medium => (warningCss, "Password has average strength.")
      | ModeratelyStrong => (successCss, "Password is very strong.")
      | VeryStrong => (successCss, "Password is very strong.")
      };
    <span className> {React.string(text)} </span>;
  | NotValidated => React.null
  };
};

let fieldCssFn = (validated: ValidationResult.t(ValidationCore.Password.t)) => {
  switch (validated) {
  | Validated(validated) =>
    switch (validated) {
    | TooShort
    | TooWeak
    | VeryWeak => " " ++ fieldFailureCss
    | Medium => " " ++ fieldWarningCss
    | ModeratelyStrong
    | VeryStrong => " " ++ fieldSuccessCss
    }
  | NotValidated => ""
  };
};

include MakeField({
  type kind = ValidationCore.Password.t;

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
