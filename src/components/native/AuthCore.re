open React.Event;

module FieldProps = {
  type t = {
    name: string,
    type_: string,
    placeholder: string,
    iconFn: unit => React.element,
  };
};

module type FieldState = {
  type t;

  let field_props: FieldProps.t;

  let field_css: option(t) => option(string);

  let field_span: option(t) => React.element;
};

module MakeField = (S: FieldState) => {
  [@react.component]
  let make = (~state, ~value, ~onChange) => {
    let field_css =
      switch (S.field_css(state)) {
      | Some(e) => "auth-field-input " ++ e
      | None => "auth-field-input"
      };
    <div className="auth-field-container">
      <div className=field_css>
        {S.field_props.iconFn()}
        <input
          name={S.field_props.name}
          type_={S.field_props.type_}
          placeholder={S.field_props.placeholder}
          required=true
          autoComplete="off"
          value
          onChange
        />
      </div>
      {S.field_span(state)}
    </div>;
  };
};

module UsernameState = {
  type t =
    | TooShort
    | TooLong
    | InvalidCharacter(string);

  let field_props: FieldProps.t = {
    name: "username",
    type_: "text",
    placeholder: "Username",
    iconFn: () => <Icons.User size="1em" />,
  };

  let render =
    fun
    | TooShort => "Must be more than 3 characters."
    | TooLong => "Must be less than 16 characters."
    | InvalidCharacter(c) =>
      Printf.sprintf("'%s' is not a valid character.", c);

  let field_css =
    fun
    | None => None
    | _ => Some("auth-field-input-error");

  let field_span =
    fun
    | None => React.null
    | Some(t) =>
      <span className="auth-field-error-message poppins-light">
        {t->render->React.string}
      </span>;
};

module UsernameField = MakeField(UsernameState);

module PasswordState = {
  type t =
    | TooShort
    | TooWeak
    | VeryWeak
    | Medium
    | ModeratelyStrong
    | VeryStrong;

  let render: t => string =
    fun
    | TooShort => "This password is too short."
    | TooWeak => "This password is too weak."
    | VeryWeak => "This password is very weak."
    | Medium => "This password has medium strength."
    | ModeratelyStrong => "This password is moderately strong."
    | VeryStrong => "This password is very strong.";

  let field_props: FieldProps.t = {
    name: "password",
    type_: "password",
    placeholder: "Password",
    iconFn: () => <Icons.LockPassword size="1em" />,
  };

  let field_css =
    fun
    | None => None
    | _ => Some("auth-field-input-error");

  let field_span =
    fun
    | None => React.null
    | Some(t) =>
      <span className="auth-field-error-message poppins-light">
        {t->render->React.string}
      </span>;
};

module PasswordField = MakeField(PasswordState);

module ConfirmState = {
  type t =
    | Yes
    | No;

  let render =
    fun
    | Yes => "Passwords match. You're good to go!"
    | No => "Passwords do not match.";

  let field_props: FieldProps.t = {
    name: "confirmPassword",
    type_: "password",
    placeholder: "Confirm Password",
    iconFn: () => <Icons.LockPassword size="1em" />,
  };

  let field_css =
    fun
    | None => None
    | Some(_) => Some("auth-field-input-error");

  let field_span =
    fun
    | None => React.null
    | Some(t) =>
      <span className="auth-field-error-message poppins-light">
        {t->render->React.string}
      </span>;
};

module ConfirmField = MakeField(ConfirmState);

[@react.component]
let make =
    (
      ~register: bool,
      ~username: option(string)=?,
      ~onUsernameChange: option(Form.t => unit)=?,
      ~password: option(string)=?,
      ~onPasswordChange: option(Form.t => unit)=?,
      ~confirmPassword: option(string)=?,
      ~onConfirmPasswordChange: option(Form.t => unit)=?,
      ~onSubmit: option(Form.t => unit)=?,
      ~usernameState: option(option(UsernameState.t))=?,
      ~passwordState: option(option(PasswordState.t))=?,
      ~confirmState: option(option(ConfirmState.t))=?,
    ) => {
  let username = Option.value(username, ~default="");
  let onUsernameChange = Option.value(onUsernameChange, ~default=_ => ());

  let password = Option.value(password, ~default="");
  let onPasswordChange = Option.value(onPasswordChange, ~default=_ => ());

  let confirmPassword = Option.value(confirmPassword, ~default="");
  let onConfirmPasswordChange =
    Option.value(onConfirmPasswordChange, ~default=_ => ());

  let onSubmit = Option.value(onSubmit, ~default=_ => ());

  let usernameState = Option.value(usernameState, ~default=None);
  let passwordState = Option.value(passwordState, ~default=None);
  let confirmState = Option.value(confirmState, ~default=None);

  <div className="auth-container">
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
    </form>
    <span className="poppins-regular">
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
  </div>;
};
