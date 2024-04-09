[@react.component]
let make = () => {
  <>
    <Link href="/inbox" className="header-button header-login">
      {React.string("Inbox")}
    </Link>
    <Link href="/compose" className="header-button header-register">
      {React.string("Compose")}
    </Link>
  </>;
};
