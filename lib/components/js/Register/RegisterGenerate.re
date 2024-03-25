[@react.component]
let make =
    (
      ~formState: RegisterFormState.t,
      ~generateState: RegisterGenerateState.t,
      ~nextRegisterStep,
    ) => {
  let _ = formState;
  let _ = generateState;
  let _ = nextRegisterStep;

  let (generateStep, nextGenerateStep) = RegisterGenerateState.useStep();
  React.useEffect1(
    () => {
      switch (generateStep) {
      | RegisterGenerateState.Finished => ()
      | _ => ignore(Js.Global.setTimeout(~f=nextGenerateStep, 1000))
      };
      None;
    },
    [|generateStep|],
  );

  <div className="register-generate-content">
    <span className="josefin-sans-title generate-title">
      {React.string("Making Your Keys")}
    </span>
    <RegisterGenerateStep generateStep />
    <p className="poppins-regular register-generate-hint">
      {React.string(
         "Once finished, your encrypted private keys, public keys, and other metadata will be submitted to the server to finalize your registration.",
       )}
    </p>
  </div>;
};
