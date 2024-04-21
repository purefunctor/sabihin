open RegisterPageFieldCore;

let successCss = [%cx {| color: $(Theme.success) |}];
let warningCss = [%cx {| color: $(Theme.warning) |}];
let failureCss = [%cx {| color: $(Theme.failure) |}];

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

include MakeField({
  type kind = ValidationCore.Password.t;

  let meta = {
    title: "Password",
    name: "password",
    type_: "password",
    placeholder: "password (min. 8 characters)",
  };
  let iconFn = _ => <Icons.BookLine size="1.5rem" />;
  let hintFn = hintFn;
});
