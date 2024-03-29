open React.Event;

type props_t = {
  name: string,
  type_: string,
  placeholder: string,
};

module type FieldState = {
  type t;

  let renderComment: t => string;

  let fieldProps: props_t;

  let fieldCss: ValidationResult.t(t) => option(string);

  let fieldIcon: React.element;

  let commentCss: t => string;
};

module MakeField = (S: FieldState) => {
  [@react.component]
  let make = (~state, ~value, ~onChange) => {
    let fieldCss =
      switch (S.fieldCss(state)) {
      | Some(e) => "auth-field-input " ++ e
      | None => "auth-field-input"
      };
    <div className="auth-field-container">
      <div className=fieldCss>
        S.fieldIcon
        <input
          name={S.fieldProps.name}
          type_={S.fieldProps.type_}
          placeholder={S.fieldProps.placeholder}
          required=true
          autoComplete="off"
          value
          onChange
        />
      </div>
      {switch (state) {
       | NotValidated => React.null
       | Validated(t) =>
         let commentCss =
           "auth-field-error-message poppins-light " ++ S.commentCss(t);
         <span className=commentCss>
           {S.renderComment(t)->React.string}
         </span>;
       }}
    </div>;
  };
};

module UsernameState = {
  include ValidationCore.Username;

  let fieldProps = {name: "username", type_: "text", placeholder: "Username"};

  let renderComment =
    fun
    | TooShort => "Must be more than 3 characters."
    | TooLong => "Must be less than 16 characters."
    | InvalidCharacter(c) =>
      Printf.sprintf("'%s' is not a valid character.", c)
    | Success => "Awesome! You're good to go!";

  let fieldCss =
    ValidationResult.map_or_none(
      fun
      | TooShort
      | TooLong
      | InvalidCharacter(_) => "auth-field-input-error"
      | Success => "auth-field-input-success",
    );

  let fieldIcon = <Icons.User size="1em" />;

  let commentCss =
    fun
    | TooShort
    | TooLong
    | InvalidCharacter(_) => "color-error"
    | Success => "color-success";
};

module UsernameField = MakeField(UsernameState);

module PasswordState = {
  include ValidationCore.Password;

  let renderComment: t => string =
    fun
    | TooShort => "Password must at least be 8 characters."
    | TooWeak => "This password is too weak."
    | VeryWeak => "This password is very weak."
    | Medium => "This password has average strength."
    | ModeratelyStrong => "This password is moderately strong."
    | VeryStrong => "This password is very strong.";

  let fieldProps = {
    name: "password",
    type_: "password",
    placeholder: "Password",
  };

  let fieldCss =
    ValidationResult.map_or_none(
      fun
      | TooShort
      | TooWeak
      | VeryWeak => "auth-field-input-error"
      | Medium => "auth-field-input-warning"
      | ModeratelyStrong
      | VeryStrong => "auth-field-input-success",
    );

  let fieldIcon = <Icons.LockPassword size="1em" />;

  let commentCss =
    fun
    | TooWeak
    | TooShort
    | VeryWeak => "color-error"
    | Medium => "color-warning"
    | ModeratelyStrong
    | VeryStrong => "color-success";
};

module PasswordField = MakeField(PasswordState);

module ConfirmState = {
  include ValidationCore.PasswordConfirm;

  let renderComment =
    fun
    | Yes => "Passwords match. You're good to go!"
    | No => "Passwords do not match.";

  let fieldProps = {
    name: "confirmPassword",
    type_: "password",
    placeholder: "Confirm Password",
  };

  let fieldCss =
    ValidationResult.map_or_none(
      fun
      | Yes => "auth-field-input-success"
      | No => "auth-field-input-error",
    );

  let fieldIcon = <Icons.LockPassword size="1em" />;

  let commentCss =
    fun
    | Yes => " color-success"
    | No => " color-error";
};

module ConfirmField = MakeField(ConfirmState);

[@react.component]
let make =
    (
      ~register: bool,
      ~username: string="",
      ~password: string="",
      ~confirmPassword: string="",
      ~onUsernameChange: Form.t => unit=_ => (),
      ~onPasswordChange: Form.t => unit=_ => (),
      ~onConfirmPasswordChange: Form.t => unit=_ => (),
      ~usernameState: ValidationResult.t(UsernameState.t)=NotValidated,
      ~passwordState: ValidationResult.t(PasswordState.t)=NotValidated,
      ~confirmState: ValidationResult.t(ConfirmState.t)=NotValidated,
      ~onSubmit: Form.t => unit=_ => (),
    ) => {
  <form onSubmit className="auth-form">
    <span className="auth-title josefin-sans-title">
      {React.string(if (register) {"Register"} else {"Login"})}
    </span>
    <UsernameField
      state=usernameState
      value=username
      onChange=onUsernameChange
    />
    <PasswordField
      state=passwordState
      value=password
      onChange=onPasswordChange
    />
    {if (register) {
       <ConfirmField
         state=confirmState
         value=confirmPassword
         onChange=onConfirmPasswordChange
       />;
     } else {
       React.null;
     }}
    <button className="auth-button button-primary poppins-regular">
      {React.string(if (register) {"Register"} else {"Login"})}
    </button>
    <span className="auth-redirect-message poppins-regular">
      {if (register) {
         <>
           {React.string("Already have an account? ")}
           <Link href="/login" className="auth-link">
             {React.string("Log in.")}
           </Link>
         </>;
       } else {
         <>
           {React.string("No account? ")}
           <Link href="/register" className="auth-link">
             {React.string("Get started.")}
           </Link>
         </>;
       }}
    </span>
  </form>;
};
