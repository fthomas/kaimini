dyn.load("fisp.so")

RpvFitInit <- function(in.file = "LesHouches.in", out.file = "SPheno.spc")
{
  .Call("rpvfit_init", list(input = in.file, output = out.file))
}

RpvFitGetParams <- function()
{
  return(.Call("rpvfit_get_params"))
}

RpvFitChiSquare <- function(v)
{
  retval <- .Call("rpvfit_chi_square", v)
  return(retval$chisq)
}

RpvFitFinish <- function()
{
  .Call("rpvfit_finish")
}


RpvFitInit()
params <- RpvFitGetParams()
minimum <- nlm(RpvFitChiSquare, params$varParams)
RpvFitFinish()

# vim: sw=2 tw=78
