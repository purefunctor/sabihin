let useSession = (): Types_universal.Definitions_t.session_kind => {
  `Guest;
};

let useRegister = () => {
  failwith("Cannot use in server code.");
};
