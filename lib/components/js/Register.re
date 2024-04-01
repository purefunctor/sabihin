[@react.component]
let make = () => {
  let formState = RegisterFormState.use();
  let generateState = RegisterGenerateState.use();

  let (registerStep, nextRegisterStep) = RegisterFormState.useStep();

  <main className="login-register-content">
    {switch (registerStep) {
     | RegisterFormState.Initial =>
       <RegisterForm formState nextRegisterStep generateState />
     | RegisterFormState.Generate =>
       <RegisterGenerate nextRegisterStep generateState />
     | RegisterFormState.Finish => <RegisterFinish formState generateState />
     }}
  </main>;
};
