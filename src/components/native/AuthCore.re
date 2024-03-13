[@react.component]
let make = (~register: bool) => {
  <div className="auth-container">
    <form className="auth-form">
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
        />
      </div>
      {if (register) {
         <div className="auth-field">
           <Icons.LockPassword size="1em" />
           <input
             name="confirmpassword"
             type_="password"
             required=true
             autoComplete="off"
             placeholder="Confirm Password"
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
