[@react.component]
let make = () => {
  let registerState = RegisterState.use();
  let (step, nextStep) = RegisterState.useStep();

  <main className="login-register-content">
    {switch (step) {
     | RegisterState.Initial => <RegisterInitial registerState nextStep />
     | RegisterState.Generate => <RegisterGenerate registerState nextStep />
     | RegisterState.Finish => <RegisterFinish registerState />
     }}
  </main>;
};
