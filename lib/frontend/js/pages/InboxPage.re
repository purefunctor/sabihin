let useIsLoading = () => {
  let (isLoading, setIsLoading) = React.useState(() => true);
  React.useEffect0(() => {
    setIsLoading(_ => false);
    None;
  });
  isLoading;
};

[@react.component]
let make = () => {
  let isLoading = useIsLoading();

  <InboxPageLayout>
    {isLoading ? <InboxPageInitial /> : <InboxPageMessages />}
  </InboxPageLayout>;
};
