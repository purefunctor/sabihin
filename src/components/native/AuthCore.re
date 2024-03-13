open React.Event;

module Field = {
  [@react.component]
  let make =
      (~name, ~type_, ~placeholder, ~value, ~onChange, ~errorText, ~iconFn) => {
    let failureClass =
      switch (errorText) {
      | Some(_) => " auth-field-input-error"
      | None => ""
      };
    <div className="auth-field-container">
      <div className={"auth-field-input" ++ failureClass}>
        {iconFn()}
        <input
          name
          type_
          required=true
          autoComplete="off"
          placeholder
          value
          onChange
        />
      </div>
      {switch (errorText) {
       | Some(errorText) =>
         <span className="auth-field-error-message poppins-light">
           {React.string(errorText)}
         </span>
       | None => React.null
       }}
    </div>;
  };
};

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
      ~usernameErrorText: option(option(string))=?,
    ) => {
  let username = Option.value(username, ~default="");
  let onUsernameChange = Option.value(onUsernameChange, ~default=_ => ());

  let password = Option.value(password, ~default="");
  let onPasswordChange = Option.value(onPasswordChange, ~default=_ => ());

  let confirmPassword = Option.value(confirmPassword, ~default="");
  let onConfirmPasswordChange =
    Option.value(onConfirmPasswordChange, ~default=_ => ());

  let onSubmit = Option.value(onSubmit, ~default=_ => ());

  let usernameErrorText = Option.value(usernameErrorText, ~default=None);

  <div className="auth-container">
    <form onSubmit className="auth-form">
      <span className="auth-title josefin-sans-title">
        {React.string(if (register) {"Register"} else {"Login"})}
      </span>
      <Field
        name="username"
        type_="text"
        placeholder="Username"
        value=username
        onChange=onUsernameChange
        iconFn={_ => <Icons.User size="1em" />}
        errorText=usernameErrorText
      />
      <Field
        name="password"
        type_="password"
        placeholder="Password"
        value=password
        onChange=onPasswordChange
        iconFn={_ => <Icons.LockPassword size="1em" />}
        errorText=None
      />
      {if (register) {
         <Field
           name="confirmPassword"
           type_="password"
           placeholder="Confirm Password"
           value=confirmPassword
           onChange=onConfirmPasswordChange
           iconFn={_ => <Icons.LockPassword size="1em" />}
           errorText=None
         />;
       } else {
         <> </>;
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
