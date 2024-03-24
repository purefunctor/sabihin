[@react.component]
let make = () => {
  let registerState = RegisterState.use();
  let generateState = RegisterGenerateState.use();

  let (registerStep, nextRegisterStep) = RegisterState.useStep();

  <main className="login-register-content">
    {switch (registerStep) {
     | RegisterState.Initial =>
       <RegisterInitial registerState nextRegisterStep />
     | RegisterState.Generate =>
       <RegisterGenerate registerState nextRegisterStep generateState />
     | RegisterState.Finish => <RegisterFinish registerState generateState />
     }}
  </main>;
};
