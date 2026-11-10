// Same pipeline as the Seeds UI subtheme kit: autoprefixer, then rtlcss for
// the .rtl.css build (NODE_ENV=RTL).
export default (context) => ({
  map: false,
  plugins: {
    autoprefixer: { cascade: false },
    rtlcss: context.env === 'RTL',
  },
});
