open React.Event;

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
    ) => {
  let username = Option.value(username, ~default="");
  let onUsernameChange = Option.value(onUsernameChange, ~default=_ => ());

  let password = Option.value(password, ~default="");
  let onPasswordChange = Option.value(onPasswordChange, ~default=_ => ());

  let confirmPassword = Option.value(confirmPassword, ~default="");
  let onConfirmPasswordChange =
    Option.value(onConfirmPasswordChange, ~default=_ => ());

  let onSubmit = Option.value(onSubmit, ~default=_ => ());

  <div className="auth-container">
    <form onSubmit className="auth-form">
      <span className="auth-title josefin-sans-title">
        {React.string(if (register) {"Register"} else {"Login"})}
      </span>
      <div className="auth-field">
        <Icons.User size="1em" />
        <input
          name="username"
          type_="text"
          required=true
          autoComplete="off"
          placeholder="Username"
          value=username
          onChange=onUsernameChange
        />
      </div>
      <div className="auth-field">
        <Icons.LockPassword size="1em" />
        <input
          name="password"
          type_="password"
          required=true
          autoComplete="off"
          placeholder="Password"
          value=password
          onChange=onPasswordChange
        />
      </div>
      {if (register) {
         <div className="auth-field">
           <Icons.LockPassword size="1em" />
           <input
             name="confirmPassword"
             type_="password"
             required=true
             autoComplete="off"
             placeholder="Confirm Password"
             value=confirmPassword
             onChange=onConfirmPasswordChange
           />
         </div>;
       } else {
         <> </>;
       }}
      <button className="auth-button button-primary poppins-regular">
        {React.string("Login")}
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
