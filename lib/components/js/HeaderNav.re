[@react.component]
let make = () => {
  let session = Session.useSession();
  <nav className="header-nav josefin-sans-regular">
    {switch (session) {
     | Loading => <HeaderNavLoading />
     | Guest => <HeaderNavGuest />
     | LoggedIn(_) => <HeaderNavLoggedIn />
     }}
  </nav>;
};
