[@react.component]
let make = () => {
  <header className="header">
    <Link href="/" className="header-title josefin-sans-title">
      <span> {React.string("sabihin")} </span>
      <span className="color-primary"> {React.string(".")} </span>
      <span> {React.string("ph")} </span>
    </Link>
    <HeaderNav />
  </header>;
};
