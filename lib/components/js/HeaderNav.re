type state =
  | Initial
  | Guest
  | LoggedIn;

[@react.component]
let make = () => {
  let (state, _) = React.useState(() => Initial);
  <nav className="header-nav josefin-sans-regular">
    {switch (state) {
     | Initial => <HeaderNavInitial />
     | Guest => <HeaderNavGuest />
     | LoggedIn => <HeaderNavLoggedIn />
     }}
  </nav>;
};
