[@react.component]
let make = () => {
  <header className="header">
    <Link href="/" className="header-title josefin-sans-title">
      <span> {React.string("sabihin")} </span>
      <span className="color-primary"> {React.string(".")} </span>
      <span> {React.string("ph")} </span>
    </Link>
    <nav className="header-nav josefin-sans-regular">
      <Link href="/login" className="header-button header-login">
        {React.string("Login")}
      </Link>
      <a className="header-button header-register">
        {React.string("Register")}
      </a>
    </nav>
  </header>;
};
