[@react.component]
let make = () => {
  <DerivedSecretsContext.Provider>
    <SessionContext.Provider> <Layout /> </SessionContext.Provider>
  </DerivedSecretsContext.Provider>;
};
