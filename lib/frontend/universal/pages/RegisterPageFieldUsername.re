open RegisterPageFieldCore;

let successCss = [%cx {| color: $(Theme.success) |}];
let failureCss = [%cx {| color: $(Theme.failure) |}];

let hintFn =
    (validation: ValidationResult.t(ValidationCore.Username.t)) => {
  switch (validation) {
  | Validated(validated) =>
    let (className, text) =
      switch (validated) {
      | TooShort => (failureCss, "Must be more than 3 characters.")
      | TooLong => (failureCss, "Must be less than 16 characters.")
      | InvalidCharacter(c) => (
          failureCss,
          Printf.sprintf("'%s' is not a valid character.", c),
        )
      | Success => (successCss, "Awesome! You're good to go.")
      };
    <span className> {React.string(text)} </span>;
  | NotValidated => React.null
  };
};

include MakeField({
  type kind = ValidationCore.Username.t;

  let meta = {
    title: "Username",
    name: "username",
    type_: "text",
    placeholder: "username (e.g. aimoto_rinku)",
  };
  let iconFn = _ => <Icons.UserLine size="1.5rem" />;
  let hintFn = hintFn;
});
