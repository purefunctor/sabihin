type loggedIn = {publicId: string};

type session =
  | Guest
  | LoggedIn(loggedIn);

type store = {
  get: unit => session,
  set: session => unit,
  subscribe: (unit => unit, unit) => unit,
};
