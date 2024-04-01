module Generate = {};

[@react.component]
let make = () => {
  let (step, toGenerate, toSubmit) = RegisterHooks.Step.use();
  <React.StrictMode>
    <main className="login-register-content">
      {switch (step) {
       | Form => <RegisterForm toGenerate />
       | Generate(generate) => <RegisterGenerate generate toSubmit />
       | Submit(submit) => <RegisterSubmit submit />
       }}
    </main>
  </React.StrictMode>;
};
