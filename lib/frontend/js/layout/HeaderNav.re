open HeaderNavHooks;

[@react.component]
let make = () => {
  let (stage, _) = Stage.use();
  switch (stage) {
  | Loading => <HeaderNavLoading />
  | Guest => <HeaderNavGuest />
  | LoggedIn => <HeaderNavLoggedIn />
  };
};
