[@react.component]
let make = () => {
  <DerivedSecretsContext.Provider>
    <GeneratedSecretsContext.Provider>
      <SessionContext.Provider> <Layout /> </SessionContext.Provider>
    </GeneratedSecretsContext.Provider>
  </DerivedSecretsContext.Provider>;
};
