[@react.component]
let make = () => {
  <div className="auth-container">
    <form className="auth-form">
      <span className="auth-title josefin-sans-title">
        {React.string("Login")}
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
      <button className="auth-button button-primary poppins-regular">
        {React.string("Login")}
      </button>
    </form>
    <span className="poppins-regular">
      {React.string("No account? ")}
      <Link href="/register" className="auth-link">
        {React.string("Get started.")}
      </Link>
    </span>
  </div>;
};
