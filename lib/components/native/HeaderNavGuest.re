[@react.component]
let make = () => {
  <>
    <Link href="/login" className="header-button header-login">
      {React.string("Login")}
    </Link>
    <Link href="/register" className="header-button header-register">
      {React.string("Register")}
    </Link>
  </>;
};
