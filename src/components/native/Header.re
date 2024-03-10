[@react.component]
let make = () => {
  <header className="header ">
    <a className="header-title josefin-sans-title">
      <span> {React.string("sabihin")} </span>
      <span className="color-secondary"> {React.string(".")} </span>
      <span> {React.string("ph")} </span>
    </a>
    <nav className="header-nav josefin-sans-regular">
      <a className="header-button header-login"> {React.string("Login")} </a>
      <a className="header-button header-register">
        {React.string("Register")}
      </a>
    </nav>
  </header>;
};
