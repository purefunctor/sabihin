[@react.component]
let make = () => {
  <ClientSecretsContext.Provider>
    <SessionContext.Provider> <Layout /> </SessionContext.Provider>
  </ClientSecretsContext.Provider>;
};
